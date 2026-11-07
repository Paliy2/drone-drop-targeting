FROM debian:12-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ cmake make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt run-all.sh ./
COPY core/ ./core/
COPY app/ ./app/

RUN cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release && cmake --build build-release

COPY input/ ./input/

RUN chmod +x run-all.sh

ENV TRAJECTORY=hover

CMD ["./run-all.sh"]
