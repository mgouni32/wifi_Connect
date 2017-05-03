#FROM resin/%%RESIN_MACHINE_NAME%%-node:6
FROM resin/raspberrypi3-debian
MAINTAINER Eric Schroeder <eric.schroeder@usaa.com>

#switch on systemd init system in container
ENV INITSYSTEM on

RUN apt-get update && apt-get install -y \
	git-core \
	build-essential \
	gcc \
	mosquitto-clients \
	libmosquitto-dev \
	monit \
	lsof \
	wget \
	--no-install-recommends && \
	rm -rf /var/lib/apt/lists/*

RUN git clone git://git.drogon.net/wiringPi
RUN cd wiringPi && ./build

COPY . /wiringPi
WORKDIR /wiringPi

#Compile ADC reading/MQTT publishing script
RUN gcc -o pub.o pub.c -lwiringPi -lmosquitto

#Compile multicolor LED script
RUN gcc -o ledstatus.o ledstatus.c -lwiringPi

RUN chmod 775 pistart.sh
#RUN chmod 775 pidlogging.sh

CMD /pistart.sh