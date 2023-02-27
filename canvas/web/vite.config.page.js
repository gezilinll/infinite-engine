// vite.config.js
import { resolve } from 'path'
import { defineConfig } from 'vite'
import topLevelAwait from 'vite-plugin-top-level-await'

export default defineConfig({
    build: {
        rollupOptions: {
            input: {
                main: resolve(__dirname, 'demo/index.html'),
                house: resolve(__dirname, 'demo/CanvasRenderingContext2D/house.html'),
                fillStyle: resolve(__dirname, 'demo/CanvasRenderingContext2D/fillStyle.html'),
                globalAlpha: resolve(__dirname, 'demo/CanvasRenderingContext2D/globalAlpha.html'),
                globalCompositeOperation: resolve(__dirname, 'demo/CanvasRenderingContext2D/globalCompositeOperation.html'),
                lineCap: resolve(__dirname, 'demo/CanvasRenderingContext2D/lineCap.html'),
                lineDashOffset: resolve(__dirname, 'demo/CanvasRenderingContext2D/lineDashOffset.html'),
                lineJoin: resolve(__dirname, 'demo/CanvasRenderingContext2D/lineJoin.html'),
                lineWidth: resolve(__dirname, 'demo/CanvasRenderingContext2D/lineWidth.html'),
                miterLimit: resolve(__dirname, 'demo/CanvasRenderingContext2D/miterLimit.html'),
                shadowBlur: resolve(__dirname, 'demo/CanvasRenderingContext2D/shadowBlur.html'),
                shadowColor: resolve(__dirname, 'demo/CanvasRenderingContext2D/shadowColor.html'),
                shadowOffsetX: resolve(__dirname, 'demo/CanvasRenderingContext2D/shadowOffsetX.html'),
                shadowOffsetY: resolve(__dirname, 'demo/CanvasRenderingContext2D/shadowOffsetY.html'),
                strokeStyle: resolve(__dirname, 'demo/CanvasRenderingContext2D/strokeStyle.html'),
            },
        },
    },
    base: "/", // resolve child directory 404 problem
    plugins: [
        topLevelAwait({
            // The export name of top-level await promise for each chunk module
            promiseExportName: "__tla",
            // The function to generate import names of top-level await promise in each chunk module
            promiseImportName: i => `__tla_${i}`
        })
    ]
})