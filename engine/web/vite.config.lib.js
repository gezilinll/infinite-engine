// vite.config.js
import { resolve } from 'path'
import { defineConfig } from 'vite'

export default defineConfig({
    build: {
        lib: {
            entry: resolve(__dirname, "src/index.ts"), // 打包的入口文件
            name: "infinite-engine", // 包名
            fileName: (format) => `infinite-engine.${format}.js` // 打包后的文件名
        },
    }
})