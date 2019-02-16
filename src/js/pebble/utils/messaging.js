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
    // queue.push(serializeMessage(message));
    queue.push(message);
}

export const flushMessages = async () => {
    // await waitMessaging();

    if (busy) {
        return;
    };

    if (queue.length > 0) {
        busy = true;

        Pebble.sendAppMessage(
            {
                batchOperations: serializeMessage(queue),
                batchOperationsSize: queue.length
            },
            () => {
                busy = false;
                console.log('Successfully sent', serializeMessage(queue), 'with ', serializeMessage(queue).length);
                queue.length = 0;
            },
            () => {
                busy = false;
                console.log('Error sending messages', queue);
            }
        );

        // var ack = () => {
        //     attempts = 0;
        //     queue.shift();
        //     busy = false;
        //     flushMessages();
        // };

        // var nack = () => {
        //     attempts++;
        //     busy = false;
        //     flushMessages();
        // };

        // if (attempts >= insistence) {
        //     console.log('Failed sending AppMessage: ' + JSON.stringify(nextMessage()));
        //     ack();
        // }
        // // console.log('Sending AppMessage: ' + JSON.stringify(nextMessage()));
        // Pebble.sendAppMessage(nextMessage(), ack, nack);
    }
}