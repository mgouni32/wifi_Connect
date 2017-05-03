# Ubuntu 16.04 with Java 8 installed.
# Build image with:  docker build -t projectpi:v1 .
FROM resin/raspberrypi3-openjdk:8u121-jre-20170325
MAINTAINER Celena Dortch <CelenaDortch@gmail.com>
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y  software-properties-common && \
    #add-apt-repository ppa:webupd8team/java -y && \
    apt-get update && \
    #echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections && \
   	#apt-get install -y oracle-java8-installer && \
    apt-get clean

COPY ProjectPi.jar /Applications/projectpi/ProjectPi.jar
COPY thresholds /Applications/projectpi/thresholds

CMD ["java","-jar","/Applications/projectpi/ProjectPi.jar"]