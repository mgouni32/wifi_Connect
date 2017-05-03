export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/host/run/dbus/system_bus_socket
sleep 1
node /usr/src/app/src/app.js --clear=true