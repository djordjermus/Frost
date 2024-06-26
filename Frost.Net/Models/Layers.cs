﻿using Frost.Interoperability;

namespace Frost.Models;

public struct Layers
{
    private ulong _value;

    public Layers() { _value = 0; }
    public Layers(ulong value) { _value = value; }

    public ulong Value { get => _value; set => _value = value; }



    public static implicit operator Layers(ulong value) =>
        new Layers(value);
    public static implicit operator ulong(Layers layers) =>
        layers._value;

    public static Layers operator |(Layers lhs, Layers rhs) =>
        new Layers(lhs._value | rhs._value);
    public static Layers operator &(Layers lhs, Layers rhs) =>
        new Layers(lhs._value & rhs._value);
    public static Layers operator ^(Layers lhs, Layers rhs) =>
        new Layers(lhs._value ^ rhs._value);

    public static Layers Default =>
        new Layers(1);
    public static Layers All =>
        new Layers(~0ul);
    public static Layers InternalLayer =>
        new Layers(FrostApi.EventSystem.GetApiBroadcastLayer());
}
