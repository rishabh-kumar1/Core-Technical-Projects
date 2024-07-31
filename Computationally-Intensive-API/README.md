**Objective:**
Design and implement a scalable job distribution system to handle high workloads efficiently.

### **Preamble:**

[] can experience variable loads, sometimes requiring the processing of thousands of computationally intensive requests per minute. To ensure robustness and responsiveness under such conditions, it is crucial to have a scalable and efficient job distribution system.

A typical scenario involves a web application backend that receives high volumes of API requests, which need to be processed quickly and efficiently. As the number of requests grows, the system must scale dynamically to maintain performance and reliability.

Implementing a scalable job distribution system enhances performance, resilience, and flexibility, effectively managing high loads. This system allows seamless addition of new worker nodes, preventing bottlenecks. Your task is to design and implement this system, demonstrating expertise in job queues, dynamic scaling, and monitoring, ensuring efficient handling of high workloads.

# Computationally intensive function

For the purposes of this, let’s assume that the computationally intensive function is a matrix multiplication function, where users can make thousands of requests per second

```python
import numpy as np

def matrix_multiplication(A, B):
    return np.dot(A, B)
```

When the size of `A` and `B` is large, the function above can take time. Running the following code takes ~20 seconds:

```python
# Example usage:
size = 100000
A = np.random.rand(size, size)
B = np.random.rand(size, size)
C = matrix_multiplication(A, B)
```

### **Tasks:**

1. **Handling Incoming Request:**
   - Build a fast api endpoint that takes in two matrices `A` and `B` (each at least of size 10,000) and adds the the job to a job queue.
2. **Job Queue Implementation:**
   - Implement a job queue system using tools like Celery, RabbitMQ, or Kafka (or any alternative equivalents you choose)
   - Ensure that jobs are enqueued and dequeued efficiently, with proper handling of job statuses (e.g., pending, in-progress, completed, failed).
   - Implement retry logic for failed jobs, ensuring that transient errors do not lead to permanent job failures.
3. **Dynamic Worker Management:**
   - Dockerize both your FastAPI app and your job queue worker. Implement a mechanism to dynamically add or remove worker instances based on the workload.
   - Use Kubernetes to manage the scaling of worker pods, configuring Horizontal Pod Autoscaler (HPA) to automatically adjust the number of worker pods based on metrics such as CPU utilization, memory usage, or custom metrics (e.g., queue length).
   - Ensure that the system can handle a burst of 1000 requests per minute, distributing these jobs across available workers effectively.

### **Deliverables:**

- **Fast api endpoint:**
  - Provide the code for a fast api endpoint that takes in the incoming request and puts it in a job queue.
- **Job Queue System Implementation:**
  - Provide the code and configuration files for the job queue system.
  - Include documentation explaining the setup, configuration, and how jobs are managed within the system.
- **Dynamic Worker Management:**
  - Provide the docker files for both your FastAPI app and job worker.
  - Provide Kubernetes configuration files, including deployment and HPA configurations.
  - Document the scaling strategy and how it ensures efficient workload distribution.

[Rishabh inputs]
Some commands that are useful in setting up, running, and testing this codebase (multiple terminals need to be used for the different ports)

source venv/bin/activate

pip install -r requirements.txt

docker build -t ‘yourdockerhubusername’/fastapi-app -f app/Dockerfile .
docker build -t ‘yourdockerhubusername’/celery-worker -f app/Dockerfile.worker .

docker push ‘yourdockerhubusername’/fastapi-app
docker push ‘yourdockerhubusername’/celery-worker

kubectl apply -f kubernetes/redis-deployment.yaml
kubectl apply -f kubernetes/rabbitmq-deployment.yaml
kubectl apply -f kubernetes/celery-deployment.yaml
kubectl apply -f kubernetes/fastapi-deployment.yaml

kubectl apply -f kubernetes/redis-service.yaml
kubectl apply -f kubernetes/rabbitmq-service.yaml
kubectl apply -f kubernetes/celery-service.yaml
kubectl apply -f kubernetes/fastapi-service.yaml

kubectl get pods
kubectl get services
kubectl logs 'pod-name' -f

minikube service fastapi-service --url

curl -X POST "http://'your-minikube-url'/matrix_multiplication/" -H "Content-Type: application/json" -d '{
"A": [[1, 2], [3, 4]],
"B": [[5, 6], [7, 8]]
}'

locust -f locustfile.py
