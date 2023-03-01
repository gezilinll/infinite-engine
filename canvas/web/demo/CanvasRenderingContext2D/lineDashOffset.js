import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.setLineDash([4, 16]);
  // Dashed line with no offset
  ctx.beginPath();
  ctx.moveTo(0, 50);
  ctx.lineTo(300, 50);
  ctx.stroke();
  // Dashed line with offset of 4
  ctx.beginPath();
  ctx.strokeStyle = 'red';
  ctx.lineDashOffset = 4;
  ctx.moveTo(0, 100);
  ctx.lineTo(300, 100);
  ctx.stroke();
  canvas.flush();

  let canvas2 = new Canvas("canvas2");
  let ctx2 = canvas2.getContext("2d");
  var offset = 0;
  function draw() {
    ctx2.clearRect(200, 0, canvas2.width, canvas2.height);
    ctx2.setLineDash([4, 2]);
    ctx2.lineDashOffset = -offset;
    ctx2.strokeRect(10, 10, 100, 100);
    canvas2.flush();
  }
  function march() {
    offset++;
    if (offset > 16) {
      offset = 0;
    }
    draw();
    setTimeout(march, 20);
  }
  march();
});