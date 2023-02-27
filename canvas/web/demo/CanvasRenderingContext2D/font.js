import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";
import { FontInfo } from "../../src/FontInfo";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");

  fetch('http://rqm1nmwwk.hn-bkt.clouddn.com/NotoSerif-Regular.ttf').then(
    (response) => response.arrayBuffer()).then(
      (buffer) => {
        // loadFont is synchronous
        let fontInfo = new FontInfo();
        fontInfo.family = "serif";
        fontInfo.style = "normal";
        fontInfo.weight = "bold";
        canvas.loadFont(buffer, fontInfo);

        ctx.font = "bold 48px serif";
        ctx.strokeText("Hello world", 50, 100);
        ctx.flush();
        ctx.delete();
      });
});