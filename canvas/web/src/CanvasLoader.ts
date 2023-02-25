//@ts-ignore
import Module from "./wasm/InfiniteCanvas";

export interface EmbindObject<T extends EmbindObject<T>> {
    clone(): T;
    delete(): void;
    deleteLater(): void;
    isAliasOf(other: any): boolean;
    isDeleted(): boolean;
}

export class CanvasLoader {
    public static module: any = undefined;
    public static wasmPath = "http://rqm1nmwwk.hn-bkt.clouddn.com/InfiniteCanvas.wasm";

    public static async init() {
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return CanvasLoader.wasmPath;
                },
            }).then((module: Module) => {
                return module;
            }).catch((err: any) => {
                console.error(err);
                return undefined;
            });

            if (instance) {
                CanvasLoader.module = instance;
                // @ts-ignore
                window.mainModule = instance;
            }
            if (CanvasLoader.module) {
                resolve(this.module);
            } else {
                reject("!!!Load Wasm Error!!!");
            }
        });
    }
}