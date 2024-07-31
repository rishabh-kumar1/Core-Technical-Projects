broker_url = 'pyamqp://guest@rabbitmq-service//'
result_backend = 'redis://redis-service:6379/0'
task_serializer = 'json'
result_serializer = 'json'
accept_content = ['json']
timezone = 'UTC'
enable_utc = True

from kombu import Exchange, Queue

task_queues = (
    Queue('default', Exchange('default'), routing_key='default'),
)

task_routes = {
    'app.tasks.matrix_multiplication_task': {'queue': 'default'},
}
