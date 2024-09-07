# for M1/M2 mac users, use this FROM instruction instead
FROM --platform=linux/amd64 ubuntu:20.04

# FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
  build-essential \  
  libssl-dev \
  zlib1g-dev

# 1 - make directory to run container (directory on container)
RUN mkdir container

# 2 - copy project into newly created directory
COPY . container

# 3 - CD into new directory for running container
WORKDIR container

# 4 - compile code
RUN make

# 5 - run command in docker container once image ran
 CMD ["./build/bin/transit_service", "1942", "apps/transit_service/web/"]
