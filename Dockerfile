FROM python:3 as python-base
COPY requirements.txt .
RUN pip install -r requirements.txt

FROM python:3-alpine3.10
COPY . /app
WORKDIR /app
COPY --from=python-base /root/.cache /root/.cache
RUN pip install -r requirements.txt && rm -rf /root/.cache
CMD python ./updateNamesiloRecord.py
