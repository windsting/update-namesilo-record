FROM ubuntu as builder
RUN apt-get update && apt-get install -y build-essential python3-pip ca-certificates && python3 -m pip install cmake conan
WORKDIR /project
COPY . .
RUN mkdir build && cd build && conan install .. && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release && cmake --build . && strip bin/update_namesilo_record

FROM scratch
COPY --from=builder /lib/x86_64-linux-gnu /lib/x86_64-linux-gnu
COPY --from=builder /lib64 /lib64
COPY --from=builder /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt
COPY --from=builder /project/build/bin/update_namesilo_record /
ENTRYPOINT ["/update_namesilo_record"]

