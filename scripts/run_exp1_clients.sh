#!/bin/sh

set -u

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <server_host> [client_count]" >&2
    exit 1
fi

HOST="$1"
CLIENTS="${2:-10}"
PIDS=""

# cleanup trap
cleanup() {
    # Stop both sides of the background pipelines: the clients and sleepers.
    pkill -TERM -P $$ 2>/dev/null
    kill $PIDS 2>/dev/null
}

trap cleanup EXIT # trap general exit
trap 'cleanup; exit 130' INT TERM # trap Ctrl-C

# Start clients
echo "Launching $CLIENTS clients against $HOST"

for i in $(seq 1 "$CLIENTS"); do
    # Send one message, then keep stdin open until the script stops the client.
    { printf 'client %s connected\n' "$i"; sleep 2147483647; } | ./bin/exp1_client "$HOST" &
    PIDS="$PIDS $!"

    sleep 0.1
done

sleep 0.5

echo
echo "Clients sent their initial messages and are still running."
echo "Type 'quit' to close all clients."
echo

while IFS= read -r COMMAND; do
    if [ "$COMMAND" = "quit" ]; then
        break
    fi

    echo "Unknown command: $COMMAND"
    echo "Type 'quit' to close all clients."
done

cleanup

for pid in $PIDS; do
    wait "$pid" 2>/dev/null
done

echo "Clients closed."
