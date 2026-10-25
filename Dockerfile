FROM debian:12-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY main.cpp run-all.sh ./
COPY include/ ./include/
COPY input/ ./input/

RUN chmod +x run-all.sh

ENV TRAJECTORY=hover

CMD ["./run-all.sh"]
