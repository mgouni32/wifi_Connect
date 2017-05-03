FROM resin/raspberrypi3-debian

RUN apt-get update && apt-get install -y python python-dev python-pip mosquitto-clients

RUN pip install paho-mqtt pathlib path.py

COPY TestDataStream.py /mqtt/TestDataStream.py
COPY Amann_Raw_epoch_csv_test /mqtt/Amann_Raw_epoch_csv_test

#CMD ["/bin/bash"]
CMD ["python", "/mqtt/TestDataStream.py"]