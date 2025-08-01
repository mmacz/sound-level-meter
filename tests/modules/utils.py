def execution_checker(result):
    """Check the return code and output for expected values."""
    assert result is not None, "Result should not be None"
    assert len(result) == 3, "Result should contain exactly three elements: (rc, stdout, stderr)"
    rc, stdout, stderr = result
    if rc != 0:
        raise AssertionError(
            f"Expected return code 0, got {rc}"
            f"\nSTDOUT: \n{stdout}"
            f"\nSTDERR: \n{stderr}"
        )

