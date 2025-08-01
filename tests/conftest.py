from pathlib import Path
import sys
import pytest

ROOT = Path(__file__).parent
sys.path.insert(0, str(ROOT / "modules"))

from modules import SLMReferenceWrapper

@pytest.fixture(scope="function")
def slm():
    """Fixture to create an SLMReferenceWrapper instance."""
    return SLMReferenceWrapper()

