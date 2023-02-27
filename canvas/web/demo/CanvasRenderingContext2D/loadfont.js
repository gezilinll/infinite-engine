import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";
import { FontInfo } from "../../src/FontInfo";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  fetch('http://rqm1nmwwk.hn-bkt.clouddn.com/Bungee-Regular.ttf').then(
    (response) => response.arrayBuffer()).then(
      (buffer) => {
        let canvas = new Canvas("canvas1");
        let ctx = canvas.getContext("2d");

        // loadFont is synchronous
        let fontInfo = new FontInfo();
        fontInfo.family = "Bungee";
        fontInfo.style = "normal";
        fontInfo.weight = "400";
        canvas.loadFont(buffer, fontInfo);

        ctx.fillStyle = '#EEE';
        ctx.fillRect(0, 0, 300, 300);
        ctx.fillStyle = 'black';
        ctx.font = '26px Bungee';
        ctx.fillText('Awesome ', 25, 100);
        ctx.flush();
        ctx.delete();
      });
});