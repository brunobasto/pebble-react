import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'
const { BatchOperationsMessage, OperationMessage } = protobuf.Root.fromJSON(protoJSON);

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

        // console.log('Queue length', queue, queue.length);

        let batchOperationMessage = BatchOperationsMessage.create({ operations: queue });

        let buffer = BatchOperationsMessage.encode(batchOperationMessage).finish();
        // let decoded = BatchOperationsMessage.decode(buffer);
        // console.log(decoded);

        var array = [];
        for (var i = 0; i < buffer.byteLength; i++) {
            array.push(buffer[i]);
        }

        Pebble.sendAppMessage(
            {
                batchOperations: array,
                batchOperationsByteLength: array.length
            },
            () => {
                busy = false;
                console.log(`Successfully sent ${array.length} bytes.`);
                queue.length = 0;
            },
            () => {
                busy = false;
                console.log('Error sending messages', array);
            }
        );
    }
}