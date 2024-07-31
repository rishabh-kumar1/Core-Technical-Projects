FROM python:3.8-slim

RUN apt-get update && apt-get install -y gcc

WORKDIR /app

COPY ../requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY . .

CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "80"]