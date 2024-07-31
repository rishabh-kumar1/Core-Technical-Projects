from celery import Celery
import numpy as np
import logging

celery = Celery('tasks')
celery.config_from_object('app.celeryconfig')

logger = logging.getLogger(__name__)

@celery.task(bind=True, max_retries=3, default_retry_delay=60)
def matrix_multiplication_task(self, A, B):
    try:
        logger.info("Starting matrix multiplication task")
        A = np.array(A)
        B = np.array(B)
        C = np.dot(A, B)
        logger.info("Matrix multiplication task completed successfully")
        return C.tolist()
    except Exception as exc:
        logger.error("Matrix multiplication task failed", exc_info=True)
        self.retry(exc=exc)


