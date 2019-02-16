const insistence = 5.
const queue = [];

let attempts = 0;
let busy = false;
let ready = false;

export const waitMessaging = () => {
    return new Promise(
        (resolve) => {
            if (ready) {
                resolve();
            }
            else {
                Pebble.addEventListener('ready', () => {
                    ready = true;

                    resolve();
                });
            }
        }
    );
}

export const isEmpty = () => {
    return queue.length === 0;
}

export const nextMessage = () => {
    return isEmpty() ? {} : queue[0];
}

const serializeMessage = (message) => {
    const json = JSON.stringify(message);

    return json;
}

export const enqueueMessage = (message) => {
    queue.push(message);
}

export const flushMessages = async () => {
    if (busy) {
        return;
    };

    if (queue.length > 0) {
        busy = true;

        const operations = serializeMessage(queue);

        Pebble.sendAppMessage(
            {
                batchOperations: operations,
                batchOperationsSize: queue.length
            },
            () => {
                busy = false;
                console.log('Successfully sent', operations, 'with ', operations.length);
                queue.length = 0;
            },
            () => {
                busy = false;
                console.log('Error sending messages', queue);
            }
        );
    }
}