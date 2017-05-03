FROM resin/raspberrypi3-debian
MAINTAINER justin@dray.be

# Let's start with some basic stuff.
RUN apt-get update && apt-get install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    lxc \
    iptables \
    vim \
    aufs-tools \
    python3-pip

RUN pip3 install awscli 
    
# Install Docker from hypriot repos
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 37BBEE3F7AD95B3F && \
    echo "deb https://packagecloud.io/Hypriot/Schatzkiste/debian/ wheezy main" > /etc/apt/sources.list.d/hypriot.list && \
    apt-get update && \
    apt-get install -y docker-hypriot docker-compose

COPY ./wrapdocker /usr/local/bin/wrapdocker
COPY ./dockerload.sh /usr/local/bin/dockerload.sh
COPY ./dockersave.sh /usr/local/bin/dockersave.sh

RUN chmod 777 /usr/local/bin/dockerload.sh
RUN chmod 777 /usr/local/bin/dockersave.sh

#COPY ./apps /apps
#WORKDIR /apps

COPY ./apps/docker-compose.yml /usr/local/bin/docker-compose.yml

# Install resin-wifi-connect

RUN mkdir -p /usr/src/app/
WORKDIR /usr/src/app
COPY package.json /usr/src/app/
RUN JOBS=MAX npm install --unsafe-perm --production \
    && npm cache clean
COPY bower.json .bowerrc /usr/src/app/
RUN ./node_modules/.bin/bower --allow-root install \
    && ./node_modules/.bin/bower --allow-root cache clean
COPY . /usr/src/app/
RUN ./node_modules/.bin/coffee -c ./src

# -- End of resin-wifi-connect section -- #

# Define additional metadata for our image.
VOLUME /var/lib/docker
ADD start /start
CMD /start
#CMD /bin/bash