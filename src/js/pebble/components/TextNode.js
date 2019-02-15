class TextNode {
    constructor(text) {
        this.text = text;
    }

    commitUpdate(newText) {
        this.text = newText;
    }
}

export default TextNode;