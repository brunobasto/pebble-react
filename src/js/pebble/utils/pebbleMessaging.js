let ready = false;

const send = (state) => {
    return new Promise(
        (resolve, reject) => {
            Pebble.sendAppMessage(
                state,
                () => resolve(),
                (reason) => {
                    console.log('Pebble.sendAppMessage rejected', reason);
                    reject();
                }
            );
        }
    );
}

const wait = () => {
    return new Promise(
        (resolve) => {
            Pebble.addEventListener('ready', () => {
                ready = true;

                resolve();
            });
        }
    );
}

const sendPebbleMessage = async (state) => {
    console.log('sendPebbleMessage'. state);

    if (ready) {
        return await send(state);
    }

    await wait();

    return await send(state);
}

export {
    sendPebbleMessage
}