# base-image for node on any machine using a template variable,
# see more about dockerfile templates here:http://docs.resin.io/pages/deployment/docker-templates
# Note the node:slim image doesn't have node-gyp
FROM resin/raspberrypi3-node:6.10

# Defines our working directory in container
RUN mkdir -p /usr/src/app/
WORKDIR /usr/src/app

# This install npm dependencies on the resin.io build server,
# making sure to clean up the artifacts it creates in order to reduce the image size.
RUN JOBS=MAX npm install -g node-red node-red-dashboard node-red-contrib-aws-iot-hub node-red-contrib-resinio && npm cache clean && rm -rf /tmp/*

# This will copy all files in our root to the working  directory in the container
COPY . /data

# Enable systemd init system in container
#ENV INITSYSTEM=on

# server.js will run when container starts up on the device
CMD ["node-red", "--userDir", "/data", "flows.json"]