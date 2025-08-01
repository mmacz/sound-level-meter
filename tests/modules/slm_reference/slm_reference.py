import yaml
import subprocess
import sys
import platform
import os

class SLMReferenceWrapper:
    def __init__(self):
        spec_path = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            "cli_spec.yml"
        )
        assert os.path.exists(spec_path), f"Specification file not found: {spec_path}"
        with open(spec_path) as f:
            self.spec = yaml.safe_load(f)
        self.tool = self.spec["tool"]
        self.args_spec = self.spec["args"]
        self._args = {}

    def __setattr__(self, name, value):
        if name in ["spec", "tool", "args_spec", "_args"]:
            object.__setattr__(self, name, value)
        elif any(arg["name"] == name for arg in self.args_spec):
            self._args[name] = value
        else:
            object.__setattr__(self, name, value)

    def run(self):
        tool = self.tool
        if platform.system() == "Windows":
            tool = f"{tool}.exe" if not tool.endswith(".exe") else tool
        script_dir = os.path.dirname(os.path.abspath(__file__))
        tool_path = os.path.join(script_dir, "bin", tool)
        assert os.path.exists(tool_path), f"Tool not found: {tool_path}"
        cmd = [ tool_path  ]
        special_args = [ "help" ]
        skip_required = any(self._args.get(arg) for arg in special_args)
        for arg in self.args_spec:
            name = arg["name"]
            flags = arg["flags"]
            if arg.get("required") and name not in self._args and not skip_required:
                raise ValueError(f"Missing required argument: {name}")
            if name in self._args:
                cmd.extend([flags, str(self._args[name])])
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        stdout, stderr = [], []
        while True:
            out_line = process.stdout.readline() if process.stdout else ""
            err_line = process.stderr.readline() if process.stderr else ""
            if out_line:
                stdout.append(out_line)
            if err_line:
                stderr.append(err_line)
            if out_line == "" and err_line == "" and process.poll() is not None:
                break
        return process.returncode, "".join(stdout), "".join(stderr)

