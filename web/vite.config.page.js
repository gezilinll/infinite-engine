// vite.config.js
import { resolve } from 'path'
import { defineConfig } from 'vite'
import topLevelAwait from 'vite-plugin-top-level-await'

export default defineConfig({
    build: {
        rollupOptions: {
            input: {
                main: resolve(__dirname, 'demo/index.html'),
                nested: resolve(__dirname, 'demo/CanvasRenderingContext2D/house.html'),
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