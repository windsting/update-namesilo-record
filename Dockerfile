ARG PYTHON_VERSION=3-alpine3.10

FROM python:${PYTHON_VERSION} as python-base
COPY requirements.txt .
RUN apk add libxml2-dev libxslt-dev python3-dev musl-dev gcc
RUN pip install -r requirements.txt

FROM python:${PYTHON_VERSION}
COPY . /app
WORKDIR /app
COPY --from=python-base /root/.cache /root/.cache
RUN apk add --no-cache libxml2 libxslt && pip install -r requirements.txt && rm -rf /root/.cache
CMD python ./updateNamesiloRecord.py
