using Frost.Interoperability;
using Frost.Net.Models;
using Frost.Synchronization;
using System.Runtime.InteropServices;

namespace Frost
{
	public sealed class Window : SynchronizationResource
    {
		private FrostApi.Procedure _internalProc;
		private Procedure<WindowEvent> _procedure;
		public Window(WindowOptions options)
		{
			_procedure = options.procedure;
			Handle = InternalCreate(options, out _internalProc);
		}

		public bool Enabled
		{
			get => FrostApi.Window.IsEnabled(Handle);
			set => FrostApi.Window.SetEnabled(Handle, value);
		}
		public bool Active
		{
			get => FrostApi.Window.IsActive(Handle);
			set => FrostApi.Window.SetActive(Handle, value);
		}
		public bool Focused
		{
			get => FrostApi.Window.IsFocused(Handle);
			set => FrostApi.Window.SetFocused(Handle, value);
		}
		public WindowState State
		{
			get => (WindowState)FrostApi.Window.GetState(Handle);
			set => FrostApi.Window.SetState(Handle, (byte)value);
		}
		public (int, int) Position
		{
			get 
			{
				var data = FrostApi.Window.GetPosition(Handle);
				return (data.x, data.y);
			}
			set => FrostApi.Window.SetPosition(Handle, new FrostApi.Point2D() { x = value.Item1, y = value.Item2 });
		}
		public (int, int) Size
		{
			get
			{
				var data = FrostApi.Window.GetSize(Handle);
				return (data.width, data.height);
			}
			set => FrostApi.Window.SetSize(Handle, new FrostApi.Size2D() { width = value.Item1, height = value.Item2 });
		}
		public Procedure<WindowEvent> Procedure
		{
			get => _procedure;
			set => _procedure = value;
		}



		private IntPtr InternalCreate(WindowOptions options, out FrostApi.Procedure procedure)
		{
			unsafe
			{
				procedure = InternalProcedure;

				var desc		= new FrostApi.Window.Description();
				desc.position	= new FrostApi.Point2D { x = options.x, y = options.y };
				desc.size		= new FrostApi.Size2D { width = options.width, height = options.height };
				desc.procedure	= Marshal.GetFunctionPointerForDelegate(procedure);
				desc.state		= (byte)options.state;

				return FrostApi.Window.Create((IntPtr)(&desc));
			}
		}

		private unsafe void InternalProcedure(IntPtr ptr)
		{
			var ptrEvent = (FrostApi.Window.WindowEvent*)ptr.ToPointer();
			var e = FormEvent(ref *ptrEvent);
			if (e is not null)
				_procedure?.Invoke(e);
		}

		private WindowEvent? FormEvent(ref FrostApi.Window.WindowEvent e)
		{
			switch (e.type)
			{
				case FrostApi.Window.WindowEvent.enable:
					return new WindowEvent.Enabled(this);
				case FrostApi.Window.WindowEvent.disable:
					return new WindowEvent.Disabled(this);
				case FrostApi.Window.WindowEvent.activate:
					return new WindowEvent.Activated(this);
				case FrostApi.Window.WindowEvent.deactivate:
					return new WindowEvent.Deactivated(this);
				case FrostApi.Window.WindowEvent.gain_focus:
					return new WindowEvent.GainedFocus(this);
				case FrostApi.Window.WindowEvent.lose_focus:
					return new WindowEvent.LostFocus(this);
				case FrostApi.Window.WindowEvent.change_state:
					return new WindowEvent.ChangeState(this);
				case FrostApi.Window.WindowEvent.move:
					return new WindowEvent.Move(this, e.position.x, e.position.y);
				case FrostApi.Window.WindowEvent.resize:
					return new WindowEvent.Resize(this, e.size.width, e.size.height);
				case FrostApi.Window.WindowEvent.key_down:
					return new WindowEvent.KeyDown(
						this,
						(Keycode)e.keyDown.key,
						Utilities.Unmanaged.StringFromUnmanagedWstr(e.keyDown.text),
						e.keyDown.repeat);
				case FrostApi.Window.WindowEvent.key_up:
					return new WindowEvent.KeyUp(
						this, 
						(Keycode)e.keyUp.key,
						Utilities.Unmanaged.StringFromUnmanagedWstr(e.keyUp.text));
				case FrostApi.Window.WindowEvent.double_click:
					return new WindowEvent.DoubleClick(
						this,
						e.doubleClick.position.x,
						e.doubleClick.position.y,
						(Keycode)e.doubleClick.key);

				case FrostApi.Window.WindowEvent.cursor_enter:
					return new WindowEvent.CursorEnter(this, e.cursorEnter.x, e.cursorEnter.y);
				case FrostApi.Window.WindowEvent.cursor_move:
					return new WindowEvent.CursorMove(this, e.cursorMove.x, e.cursorMove.y);
				case FrostApi.Window.WindowEvent.cursor_leave:
					return new WindowEvent.CursorLeave(this, e.cursorLeave.x, e.cursorLeave.y);
				case FrostApi.Window.WindowEvent.mouse_move:
					return new WindowEvent.MouseMove(this, e.mouseMove.x, e.mouseMove.y);
				case FrostApi.Window.WindowEvent.mouse_scroll:
					return new WindowEvent.MouseScroll(this, e.mouseScroll.x, e.mouseScroll.y);
				case FrostApi.Window.WindowEvent.create:
					return new WindowEvent.Destroy(this);
				case FrostApi.Window.WindowEvent.close:
					return new WindowEvent.Close(this);
				case FrostApi.Window.WindowEvent.destroy:
					return new WindowEvent.Destroy(this);

				default:
				case FrostApi.Window.WindowEvent.invalid:
					return null;
			}
		}
	}
}
