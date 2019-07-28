FROM python:alpine3.7
COPY . /app
WORKDIR /app
RUN apk add libxslt-dev libxml2-dev
RUN pip install -r requirements.txt
CMD python ./updateNamesiloRecord.py