FROM alpine
COPY . /app
RUN apk add --no-cache curl libxml2-utils
CMD sh /app/updateNamesiloRecord.sh
