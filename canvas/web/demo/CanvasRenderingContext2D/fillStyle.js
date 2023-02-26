import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("isurface");
  let ctx = canvas.getContext("2d");

  ctx.fillStyle = 'blue';
  ctx.fillRect(10, 10, 100, 100);

  for (var i = 0; i < 6; i++) {
    for (var j = 0; j < 6; j++) {
      ctx.fillStyle = 'rgb(' + Math.floor(255 - 42.5 * i) + ',' +
        Math.floor(255 - 42.5 * j) + ',0)';
      ctx.fillRect(150 + j * 25, i * 25, 25, 25);
    }
  }

  canvas.delete();
});