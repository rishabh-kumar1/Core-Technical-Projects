from locust import HttpUser, task, between
import numpy as np
import logging

# Set up logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class MatrixMultiplicationUser(HttpUser):
    host = "http://127.0.0.1:65369"  # Replace with your actual FastAPI service URL
    wait_time = between(1, 5)  # Wait 1-5 seconds between tasks

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        # Generate smaller matrices for testing
        self.A = np.random.rand(100, 100).tolist()
        self.B = np.random.rand(100, 100).tolist()
        logger.info("Matrices generated")

    @task
    def matrix_multiplication(self):
        payload = {
            "A": self.A,
            "B": self.B
        }
        logger.info("Sending matrix multiplication request")
        with self.client.post("/matrix_multiplication/", json=payload, catch_response=True) as response:
            if response.status_code == 200:
                logger.info(f"Request successful: {response.text}")
                response.success()
            else:
                logger.error(f"Request failed with status code: {response.status_code}")
                response.failure(f"Failed with status code: {response.status_code}")

    def on_start(self):
        logger.info("A new user has been spawned")