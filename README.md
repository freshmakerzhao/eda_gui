如果想打包发布，编译时添加该选项，主程序只在同级目录下寻找resource，避免其他resource干扰

```
-DPACK_DEPLOYED=ON
```

如果仅编译Gridview，编译时添加该选项

```
-DONLY_COMPILE_GRIDVIEW=ON
```

