# Tei

🥕 A small game engine written in C++

This engine is mostly based around SDL 2. Countless C++20 features have also been used.

## Client

### Start and finish

`main` should not be defined by the client.

When the engine is ready, it will call the client implemented function `void TeiClientInit()`.
After the quit signal has been received and the engine is cleaning itself up, the user implemented `void TeiClientQuit()` is called.

It is important the function returns when the client has loaded it's resources. When returned, the engine will commence its game loop and update sequence.

An example of these function can be as follows:
```c++
// main.cpp

void TeiClientInit()
{
  puts("My game starting...");
  MyGame::Load();
}

void TeiClientQuit()
{
  puts("My game cleaning up...");
  MyGame::Cleanup();
}
```

All statics, services and resources provided and managed by the engine will be detroyed after `void TeiClientQuit()` has finsihed running.

## Services

### Scenes

This is the root of the object tree. `tei::Scenes` allows scenes to be added and switched. Scenes are functionally equivilant to a GameObject without a parent.

### Time

global time and frame information is provided by the `tei::Time` static.
Types and functions can be found in the `tei::Clock` class.

`Clock::time_point` and `Clock::duration` hold their values in seconds. Their epoch is the start of the program.

```c++
  // Time point the frame is currently in, this is the recommended time value to use.
  tei::Time->frame.now
  // Duration since last frame.
  tei::Time->frame.delta
```

Literals are also provided: days `_d`, hours `_h`, minutes `_m`, seconds `_s`, milliseconds `_ms`, microseconds `_us`, nanoseconds `_ns`.

```c++
  // Constructs a Clock::duration of 100 milliseconds
  if (tei::Time->frame.lag > 100_ms)
```

### Resources

Resources can be loaded through `tei::Resources`. Loaded resources will be managed for the client, but it is usually required to keep track of the resource when it is needed for use. 
Types can be found under `tei::resources`.

Loading the same resource twice will also load it twice.

```c++
  // Loads the sprite.png and returns a reference
  auto& gameSprite{ tei::Resources->Load<tei::resources::Texture>("./resources/sprite.png") };
```

### Renderer and Audio

Textures and sprites may be rendered through `tei::Renderer` and audio may be played through `tei::Audio`.

```c++
void MyComponent::OnEnable()
{
  // ...
  tei::Audio->Play(m_MySound);
  // ...
}
void MyComponent::OnRender()
{
  // ...
  tei::Renderer->DrawTexture(m_MyTexture, m_Transform);
  // ...
}
```

### Events

Events can de dispatched through `tei::Events`

### Application

The application can be accesses through `tei::Application`. Commandline arguments can also be found here.

To close the application: call `tei::Application->Quit()`. This will initiate cleanup.

## GameObjects and Components

### `GameObject`

`GameObject` is a container that is attached to a single parent GameObject and can have multiple children GameObjects. It can also be given `Components`.
This essentially forms the tree through which the game is updated and rendered.

To add a new child to a GameObject:
```c++
auto& child = object.AddChild();
```
The child object is attached and ready to use. Do not forget to `Enable()` the child.

When a game object is enabled/disabled, it will still be active during the current frame. As of the next frame, it will be disabled.
Disabling/Enabling am object also disables/enables its children.

When a child is added to an active object, it will be held asside until the next frame.

### `Component`

`Component`s will house the bulk of the logic and are the main powerhouses of your game. They provide callbacks like: `OnEnable`, `OnUpdate`, `OnRender` and `OnDisable`.

Components are meant to be derived from, These derived Components can be added used in GameObjects.

An example of a component may be:
```c++
class MyTimer : public tei::Component
{

  // virtual functions called by the engine
  // Any of these are optional, not providing them will simply skip the calls
  virtual void OnEnable() override;
  virtual void OnDisable() override;
  virtual void OnUpdate() override;
  virtual void OnRender() const override;
  
  // Components may be given data members
  tei::Clock::time_point m_Time{};
  tei::resources::Texture m_DialTexture{};

}
```

A component is required to have a default constructor; i.e. nothing other than an empty constructor can used. Providing no constructor is also possible.

To add a component to a GameObject, the object MUST be disabled.
```c++
auto& myTimer = object.AddComponent<MyTimer>();
```
The component is constructed, attached and ready for use. Don't forget to enable the parent object.

### Update sequence

Which object will be updated before which is currently unspecified. The pattern may be consistent but one should not rely on this being the case.

No active resources, objects or components are altered in state during a frame. They are only altered after the whole update sequence has finished.

### Clearing

To clear up an object, `Clear()` can be called on a disabled object.
Alternatively, `Disable(true)` can be called on active objects. This will both mark the object to be disabled and cleaned.

When an object is cleared, it will dispose of its children and components in respective order during the next frame.

## Other

### Text

A premade text component is proveded under `tei::prefab::TextComponent`.

# Tei 

<img src="https://en.touhouwiki.net/images/f/f5/InabaoftheMoonandInabaoftheEarthTewi.png" alt="てゐ" height="96px">
