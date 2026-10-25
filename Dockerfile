FROM debian:12-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY main.cpp run-all.sh ./

RUN g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o drop_calc

COPY input/ ./input/

RUN chmod +x run-all.sh

CMD ["./run-all.sh"]
