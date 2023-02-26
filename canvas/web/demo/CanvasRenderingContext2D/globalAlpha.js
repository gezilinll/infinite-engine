import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.globalAlpha = 0.5;
  ctx.fillStyle = 'blue';
  ctx.fillRect(10, 10, 100, 100);
  ctx.fillStyle = 'red';
  ctx.fillRect(50, 50, 100, 100);
  ctx.flush();
  canvas.delete();

  let canvas2 = new Canvas("canvas2");
  let ctx2 = canvas2.getContext("2d");
  // Draw background
  ctx2.fillStyle = '#FD0';
  ctx2.fillRect(0, 0, 75, 75);
  ctx2.fillStyle = '#6C0';
  ctx2.fillRect(75, 0, 75, 75);
  ctx2.fillStyle = '#09F';
  ctx2.fillRect(0, 75, 75, 75);
  ctx2.fillStyle = '#F30';
  ctx2.fillRect(75, 75, 75, 75);
  ctx2.fillStyle = '#FFF';
  // Set transparency value
  ctx2.globalAlpha = 0.2;
  // Draw transparent circles
  for (let i = 0; i < 7; i++) {
    ctx2.beginPath();
    ctx2.arc(75, 75, 10 + 10 * i, 0, Math.PI * 2, true);
    ctx2.fill();
  }
  ctx2.flush();
  canvas2.delete();

});