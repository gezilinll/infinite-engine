![infinite-engine](logo.jpg)

[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/gezilinll/Infinite-engine/blob/main/README.en.md)
[![MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/gezilinll/infinite-engine/blob/main/LICENSE)

# Infinite-engine

``infinite-engine`` 是一个可运行在**多个平台**上的用于**类无限画布**场景的 SDK。该项目主要由 ``infinite-engine`` 自身以及 ``infinite-canvas`` 组成，目前还在落地 Canvas 阶段。

Canvas 的 API 完全参考 Web Canvas API 设计，在 Web 平台使用时可以以近乎零成本的时候直接切换到 infinite-canvas ，其他平台则会依照特性做一些调整，但整体风格仍会最大程度保持一致。

Engine 将进一步封装类似元素等更场景化的数据模型与能力，并实现场景管理、遮挡剔除、分片缓存等无限画布所需具备的特性，并借由 Canvas 来实现跨平台渲染。

<font color=#999999>目前 Canvas 的底层直接使用的 [Google 的 SKIA](https://skia.org/docs/) 组件。 Canvas 主要起到标准层的作用，后续将进一步尝试使用 WGPU（渲染引擎方向）、Godot（游戏引擎方向）、Web Canvas（原生画布方向）等多种方案，以寻求最佳的技术选型或能够在不同场景下提供到最优解。</font>

# 接入说明

## Web

```js
// 安装
npm i [--save-dev] ez-infinite-canvas

// 导入(TODO .d.ts 类型声明)
import { CanvasLoader, Canvas } from "ez-infinite-canvas/infinite-canvas.es";

// 等待 WASM 初始化完成后即可正常使
CanvasLoader.init().then(async () => {
	let canvas = new Canvas("canvaID");
  let context2D = canvas.getContext("2d");
  ...
});
```

# API 文档与范例

## infinite-canvas

目前画布的文档展示结构直接对齐 MDN 相应文档的类目，可以访问下方地址获取详细的 API 说明、范例以及效果展示：

* [infinite-canvas(目前 WASM 未优化，需要有加载时间)](http://canvas.gezilinll.com/)
* 页面目前处于相当粗糙的起步阶段，后续将进一步优化并根据项目支持的能力逐步增加更丰富的范例

# 无限的范围

该项目旨在实现：

* 支持 2D/3D 场景渲染
* 在覆盖各平台原生所具备的能力之外，支持更多易于使用的特效
* 支持无数量上限的多种类元素渲染，支持无上限的画布尺寸
* 保持视觉质量不变的任意比例缩放（位图除外）
* 极高的稳定性与极流畅的体验
* 任意平台均可拆箱即用
* 提供配套的 API 说明、使用范例、原型展示

以下内容不在该项目的实现范围内：

* 多人协作
* 网络请求
* 类 Figma 的产品化的富编辑器
* 动画、视频等动态元素

# 支持的能力与平台

* :white_check_mark:：已支持
* :ok:：部分支持，具体见对应栏目说明
* 留空：正在拼命开发中...

## CanvasRenderingContext2D

### 实例属性

| API             | Web                    | Android | iOS  | Windows | Mac  | Linux |
| --------------- | ---------------------- | ------- | ---- | ------- | ---- | ----- |
| direction       |                        |         |      |         |      |       |
| fillStyle       | :ok:（仅支持颜色类型） |         |      |         |      |       |
| filter          |                        |         |      |         |      |       |
| font            |                        |         |      |         |      |       |
| fontKerning     |                        |         |      |         |      |       |
| fontStretch     |                        |         |      |         |      |       |
| fontVariantCaps |                        |         |      |         |      |       |
| globalAlpha     | :white_check_mark:     |         |      |         |      |       |
| shadowBlur      | :white_check_mark:     |         |      |         |      |       |
| shadowColor     | :white_check_mark:     |         |      |         |      |       |
| shadowOffsetX   | :white_check_mark:     |         |      |         |      |       |
| shadowOffsetY   | :white_check_mark:     |         |      |         |      |       |
| strokeStyle     | :ok:（仅支持颜色类型） |         |      |         |      |       |
| font            |                        |         |      |         |      |       |
| ...             |                        |         |      |         |      |       |

### 实例方法

| API       | Web                | Android | iOS  | Windows | Mac  | Linux |
| --------- | ------------------ | ------- | ---- | ------- | ---- | ----- |
| beginPath | :white_check_mark: |         |      |         |      |       |
| closePath | :white_check_mark: |         |      |         |      |       |
| fillRect  | :white_check_mark: |         |      |         |      |       |
| fillText  | :white_check_mark: |         |      |         |      |       |
| lineTo    | :white_check_mark: |         |      |         |      |       |
| drawImage |                    |         |      |         |      |       |
| ...       |                    |         |      |         |      |       |

# 项目运行说明

## Infinite-Canvas

### Web

#### 编译 WASM

在 `canvas/web` 目录下执行 `sh build_wasm_script/build.sh` 即可

* 该脚本执行前需要先本地配置 [emsdk](https://emscripten.org/docs/getting_started/downloads.html) 环境，推荐使用 3.1.15 版本

#### 本地运行调试工程

在 `canvas/web` 目录下执行 `npm run dev` 后访问 `http://127.0.0.1:5173/demo/index.html` 即可

# 项目运行数据 

## 包体积

TBD

## 质量测试

TBD

## 性能测试

TBD

# 贡献你的代码/建议

欢迎对图形渲染或本项目感兴趣的你参与到项目开发中，或者在 [Issues](https://github.com/gezilinll/infinite-engine/issues) 中提交一切你认为存在的问题或可改进的地方～
