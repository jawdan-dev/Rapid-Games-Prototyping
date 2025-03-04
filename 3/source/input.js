class InputManager {
    constructor() {
        this.keysDown = {};
        this.keysPressed = {};
        this.keysUp = {};

        this.mousePosition = new Vector2(0, 0);
        this.mouseButtonDown = {};
        this.mouseButtonPressed = {};
        this.mouseButtonUp = {};
        this.mouseScroll = 0;
    }

    keyDown(key) {
        key = key.toUpperCase();
        return key in this.keysDown;
    }
    keyPressed(key) {
        key = key.toUpperCase();
        return key in this.keysPressed;
    }
    keyUp(key) {
        key = key.toUpperCase();
        return key in this.keysUp;
    }
    mouseDown(mb) {
        return mb in this.mouseButtonDown;
    }
    mousePressed(mb) {
        return mb in this.mouseButtonPressed;
    }
    mouseUp(mb) {
        return mb in this.mouseButtonUp;
    }

    clear() {
        this.keysDown = {};
        this.keysUp = {};
        this.mouseButtonDown = {};
        this.mouseButtonUp = {};
        this.mouseScroll = 0;
    }
};
const inputManager = new InputManager();

window.addEventListener("keydown", ev => {
    const uKey = ev.key.toUpperCase();
    inputManager.keysDown[uKey] = true;
    inputManager.keysPressed[uKey] = true;
	ev.preventDefault();
})
window.addEventListener("keyup", ev => {
    const uKey = ev.key.toUpperCase();
    delete inputManager.keysPressed[uKey];
    inputManager.keysUp[uKey] = true;
	ev.preventDefault();
})

window.addEventListener("mousemove", ev => {
    inputManager.mousePosition.x = ev.clientX;
    inputManager.mousePosition.y = ev.clientY;
	ev.preventDefault();
});
window.addEventListener("mousedown", ev => {
    const mb = ev.button;
    inputManager.mouseButtonDown[mb] = true;
    inputManager.mouseButtonPressed[mb] = true;
	ev.preventDefault();
});
window.addEventListener("mouseup", ev => {
    const mb = ev.button;
    delete inputManager.mouseButtonPressed[mb];
    inputManager.mouseButtonUp[mb] = true;
	ev.preventDefault();
});
window.addEventListener("wheel", ev => {
    inputManager.mouseScroll += ev.deltaY * -0.01;
});