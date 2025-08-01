import pytest
from modules.utils import execution_checker

def test_app_run_help(slm):
    """Test the app run with help argument."""
    slm.help = True
    execution_checker(slm.run())

