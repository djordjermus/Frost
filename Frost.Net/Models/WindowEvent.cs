namespace Frost.Net.Models;

public class WindowEvent
{
	public readonly Window target;
	public WindowEvent(Window target)
	{
		this.target = target;
	}

	public class Enabled : WindowEvent
	{
		public Enabled(Window target) : base(target) { }
	}
	public class Disabled : WindowEvent
	{
		public Disabled(Window target) : base(target) { }
	}

	public class Activated : WindowEvent
	{
		public Activated(Window target) : base(target) { }
	}
	public class Deactivated : WindowEvent
	{
		public Deactivated(Window target) : base(target) { }
	}

	public class GainedFocus : WindowEvent
	{
		public GainedFocus(Window target) : base(target) { }
	}
	public class LostFocus : WindowEvent
	{
		public LostFocus(Window target) : base(target) { }
	}

	public class ChangeState : WindowEvent
	{
		public ChangeState(Window target) : base(target) { }
	}

	public class Move : WindowEvent
	{
		public readonly (int x, int y) position;
		public Move(Window target, int x, int y) : base(target)
		{
			position = (x, y);
		}
	}

	public class Resize : WindowEvent
	{
		public readonly (int width, int height) size;
		public Resize(Window target, int width, int height) : base(target)
		{
			size = (width, height);
		}
	}

	public class Cursor : WindowEvent
	{
		public readonly (int x, int y) position;
		public Cursor(Window target, int x, int y) : base(target)
		{
			position = (x, y);
		}
	}
	public class CursorEnter : Cursor
	{
		public CursorEnter(Window target, int x, int y) : base(target, x, y) { }
	}
	public class CursorMove : Cursor
	{
		public CursorMove(Window target, int x, int y) : base(target, x, y) { }
	}
	public class CursorLeave : Cursor
	{
		public CursorLeave(Window target, int x, int y) : base(target, x, y) { }
	}

	public class MouseMove : WindowEvent
	{
		public readonly (int x, int y) delta;
		public MouseMove(Window target, int dx, int dy) : base(target)
		{
			delta = (dx, dy);
		}
	}

	public class DoubleClick : WindowEvent
	{
		public readonly (int x, int y) position;
		public readonly Keycode button;
		public DoubleClick(Window target, int x, int y, Keycode button) : base(target)
		{
			position = (x, y);
			this.button = button;
		}
	}

	public class KeyDown : WindowEvent
	{
		public readonly Keycode key;
		public readonly string text;
		public readonly bool repeat;
		public KeyDown(Window target, Keycode key, string text, bool repeat) : base(target)
		{
			this.key = key;
			this.text = text;
			this.repeat = repeat;
		}
	}

	public class KeyUp : WindowEvent
	{
		public readonly Keycode key;
		public readonly string text;
		public KeyUp(Window target, Keycode key, string text) : base(target)
		{
			this.key = key;
			this.text = text;
		}
	}

	public class MouseScroll : WindowEvent
	{
		public readonly (int x, int y) delta;
		public MouseScroll(Window target, int dx, int dy) : base(target)
		{
			delta = (dx, dy);
		}
	}


	public class Create : WindowEvent
	{
		public Create(Window target) : base(target) { }
	}
	public class Close : WindowEvent
	{
		public Close(Window target) : base(target) { }
	}

	public class Destroy : WindowEvent
	{
		public Destroy(Window target) : base(target) { }
	}
}
