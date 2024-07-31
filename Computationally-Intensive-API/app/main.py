from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse
from pydantic import BaseModel
from celery.result import AsyncResult
from prometheus_client import start_http_server, Summary
from .tasks import matrix_multiplication_task
import numpy as np
import logging
logging.basicConfig(level=logging.INFO)

app = FastAPI()

class MatrixRequest(BaseModel):
    A: list
    B: list

@app.post("/matrix_multiplication/")
async def matrix_multiplication_endpoint(request: MatrixRequest):
    logging.info("Received matrix multiplication request")
    try:
        task = matrix_multiplication_task.delay(request.A, request.B)
        logging.info(f"Task created with ID: {task.id}")
        return JSONResponse(content={"task_id": str(task.id)})
    except Exception as e:
        logging.error(f"Error creating task: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/status/{task_id}")
async def get_status(task_id: str):
    task = AsyncResult(task_id)
    if task.state == 'PENDING':
        return {"status": "Pending"}
    elif task.state == 'STARTED':
        return {"status": "In Progress"}
    elif task.state == 'SUCCESS':
        return {"status": "Completed", "result": task.result}
    elif task.state == 'FAILURE':
        return {"status": "Failed", "result": str(task.result)}
    else:
        return {"status": task.state}

@app.on_event("startup")
async def startup_event():
    start_http_server(8000)  # Start Prometheus client on port 8000

@app.get("/health")
def read_health():
    return {"status": "ok"}

@app.get("/test-celery")
def test_celery():
    logging.info("Testing Celery connection")
    try:
        result = matrix_multiplication_task.delay([[1, 2], [3, 4]], [[1, 2], [3, 4]])
        logging.info(f"Test task created with ID: {result.id}")
        return {"message": "Test task created", "task_id": str(result.id)}
    except Exception as e:
        logging.error(f"Error creating test task: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))
