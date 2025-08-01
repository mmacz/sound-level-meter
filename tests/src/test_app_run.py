import pytest

def test_app_run_help(slm):
    """Test the app run with help argument."""
    slm.help = True
    slm.run()

