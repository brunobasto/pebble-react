import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'
import { Operations } from './constants.js';
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

const getParentMessage = (message, queue) => {
    return queue.find(m => m.nodeId === message.parentNodeId);
}

const reorderMessageQueue = (queue) => {
    const added = {};
    const newQueue = [];

    for (const index in queue) {
        const message = queue[index];

        if (message.operation === Operations.appendChild) {
            const parentMessage = getParentMessage(message, queue);

            if (parentMessage && !added[parentMessage.nodeId]) {
                newQueue.push(parentMessage);
                added[parentMessage.nodeId] = true;
            }

            if (!added[message.nodeId]) {
                newQueue.push(message);
                added[message.nodeId] = true;
            }
        }
        else {
            newQueue.push(message);
        }
    }

    return newQueue;
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

        let batchOperationMessage = BatchOperationsMessage.create({ operations: reorderMessageQueue(queue) });

        let buffer = BatchOperationsMessage.encode(batchOperationMessage).finish();
        let decoded = BatchOperationsMessage.decode(buffer);
        console.log(decoded);

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