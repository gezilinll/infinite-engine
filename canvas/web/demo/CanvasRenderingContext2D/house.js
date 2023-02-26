import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("isurface");
  let context2D = canvas.getContext("2d");

  // Set line width
  context2D.lineWidth = 10;
  // Wall
  context2D.strokeRect(75, 140, 150, 110);
  // Door
  context2D.fillRect(130, 190, 40, 60);
  // Roof
  context2D.beginPath();
  context2D.moveTo(50, 140);
  context2D.lineTo(150, 60);
  context2D.lineTo(250, 140);
  context2D.closePath();
  context2D.stroke();

  // ---------------- Render Example Code ---------------- 
  let startX = 300, startY = 50, lineHeight = 20;
  context2D.fillText("The code below is rendered by infinite-canvas, so you can't copy it. ^_^", startX, startY);

  startY += lineHeight;
  context2D.fillText("---------- WEB house.js----------", startX, startY += lineHeight);
  context2D.fillText("let canvas = new Canvas(\"isurface\");", startX, startY += lineHeight);
  context2D.fillText("let context2D = canvas.getContext(\"2d\");", startX, startY += lineHeight);
  context2D.fillText("context2D.lineWidth = 10;", startX, startY += lineHeight);
  context2D.fillText("context2D.strokeRect(75, 140, 150, 110);", startX, startY += lineHeight);
  context2D.fillText("context2D.fillRect(130, 190, 40, 60);", startX, startY += lineHeight);
  context2D.fillText("context2D.beginPath();", startX, startY += lineHeight);
  context2D.fillText("context2D.moveTo(50, 140);", startX, startY += lineHeight);
  context2D.fillText("context2D.lineTo(150, 60);", startX, startY += lineHeight);
  context2D.fillText("context2D.lineTo(250, 140);", startX, startY += lineHeight);
  context2D.fillText("context2D.closePath();", startX, startY += lineHeight);
  context2D.fillText("context2D.stroke();", startX, startY += lineHeight);
  context2D.fillText("context2D.delete();", startX, startY += lineHeight);

  startY += lineHeight;
  context2D.fillText("---------- Android TBD ----------", startX, startY += lineHeight);
  startY += lineHeight;
  context2D.fillText("---------- iOS TBD ----------", startX, startY += lineHeight);

  context2D.flush();
  canvas.delete();
});