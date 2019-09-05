FROM ruby:2-alpine AS builder

WORKDIR /app

ARG BUNDLER_ARGS="--jobs=8 --without test development"

COPY Gemfile* ./

RUN apk add --no-cache build-base && bundle

## built

FROM ruby:2-alpine

WORKDIR /app

COPY --from=builder /usr/local/bundle /usr/local/bundle

COPY update_namesilo_record.rb /app/

COPY lib /app/lib

CMD ["ruby", "update_namesilo_record.rb"]
