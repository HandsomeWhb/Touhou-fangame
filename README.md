# Touhou-fangame

本游戏支持弹幕、敌人、行动方式的数据驱动，以下为说明。

**游戏区域为 (320,0) 到 (1600,1600)，左上角为原点，右为 x 轴，下为 y 轴**

------

### 1. 弹幕的数据驱动

在 `assets/data/danmaku` 目录下编写 json 文件。支持两种格式。

**注意：json 文件中不能写注释！**

**(1) 随机弹幕格式：**

```
json复制编辑{
  "generate_random": {
    "angle_start": 0.0,         // 角度范围起始，向下为 0 度，顺时针递增
    "angle_end": 360.0,         // 角度范围结束
    "speed": 5.0,               // 弹幕基准速度
    "speed_variation": 1.0,     // 速度波动范围（上下浮动，比如4~6）
    "total_bullets": 100,       // 弹幕总数
    "type": "fixed",            // 类型："fixed" 固定弹，"aim" 自机狙，"track" 追踪弹（此模式推荐 fixed）
    "frame_start": 0,           // 弹幕开始帧（相对调用时帧数，60帧=1秒）
    "frame_end": 300,           // 弹幕结束帧
    "remove_on_death": false    // 敌人死亡是否清除弹幕
  }
}
```

**(2) 自定义弹幕格式：**

顺序可任意，没写的字段按默认值处理。

```
json复制编辑[
  {
    "frame": 0,                 // *必须写，弹幕开始帧
    "type": "fixed",            // 默认 fixed，支持 "fixed", "aim", "trace"
    "angle": 45.0,              // 默认0，射击角度/偏移角
    "speed": 3.0,               // 默认0，弹幕速度
    "position_x": 400.0,        // 默认0，相对敌人的发射坐标
    "position_y": 300.0,
    "aim_offset_x": 0.0,        // 默认0，瞄准偏移
    "aim_offset_y": 0.0,
    "exist_time": 9999,         // 默认9999，弹幕存在帧数
    "remove_on_death": false    // 默认 false，敌人死后是否消失
  },
  ...
]
```

------

### 2. 行为模式的数据驱动

在 `assets/data/move` 目录下编写 json，**必须按时间顺序**。

```
json复制编辑[
  {
    "frame": 2,     // 出场帧
    "angle": 90.0,  // 移动角度，向下为0度，顺时针增加
    "speed": 3.0    // 移动速度
  },
  ...
]
```

------

### 3. 敌人的数据驱动

在 `assets/data/enemy/v2` 目录下编写 json，顺序不限。

```
json复制编辑[
  {
    "num": 3,                 // 生成该波敌人数
    "appear_frame": 600,      // 敌人从第600帧开始出现
    "frame_interval": 20,     // 每个敌人间隔20帧出现
    "enemy": {
      "type": "Red_enemy",    // 敌人类型
      "hp": 200.0,            // 血量
      "x": 960.0,             // 出现位置
      "y": 400.0,
      "rewards": {            // 可选，未写则随机掉落
        "power": 1,
        "big_power": 0,
        "bomb_up": 0,
        "health_up": 1,
        "blue_point": 5
      }
    },
    "fire_plan": [
      {
        "remove_on_death": true,        // 优先级高于弹幕文件内配置
        "danmaku_offset_angle": 45,     // 整体旋转角度
        "danmaku_start_frame": 60,      // 第几帧发射弹幕
        "shoot_logic": "test.json",     // 使用的弹幕文件名
        "type": "Star",                 // 弹幕形状
        "global_speed": 4,              // 全局速度覆盖
        "color": {
          "r": 255,
          "g": 0,
          "b": 0,
          "a": 255
        }
      },
      ...
    ],
    "move": "move.json"  // 使用的移动逻辑文件
  }
]
```
