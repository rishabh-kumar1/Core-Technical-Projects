import numpy as np
import requests
import time
import logging
import json

# Set up logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# FastAPI service URL
URL = "http://127.0.0.1:65369"  # Replace with your actual FastAPI service URL

def test_large_matrix_multiplication(size=10000):
    # Generate matrices
    logger.info(f"Generating {size}x{size} matrices...")
    A = np.random.rand(size, size).tolist()
    B = np.random.rand(size, size).tolist()
    logger.info("Matrices generated")

    payload = {
        "A": A,
        "B": B
    }

    logger.info(f"Payload size: {len(json.dumps(payload)) / (1024 * 1024):.2f} MB")

    logger.info("Sending request...")
    start_time = time.time()
    
    try:
        response = requests.post(
            f"{URL}/matrix_multiplication/",
            json=payload,
            timeout=300  # 5 minutes timeout
        )
        end_time = time.time()

        if response.status_code == 200:
            task_id = response.json().get("task_id")
            logger.info(f"Request successful. Task ID: {task_id}")
            logger.info(f"Time taken: {end_time - start_time:.2f} seconds")

            # Check task status
            while True:
                status_response = requests.get(f"{URL}/status/{task_id}")
                status = status_response.json()
                logger.info(f"Task status: {status}")
                if status['status'] in ['Completed', 'Failed']:
                    break
                time.sleep(5)  # Wait for 5 seconds before checking again
        else:
            logger.error(f"Request failed with status code: {response.status_code}")
            logger.error(f"Response content: {response.text}")
    except Exception as e:
        logger.error(f"An error occurred: {str(e)}")

if __name__ == "__main__":
    test_large_matrix_multiplication(size=1000)  # Start with a smaller size
    # If successful, gradually increase the size
    # test_large_matrix_multiplication(size=5000)
    # test_large_matrix_multiplication(size=10000)