﻿namespace Frost.Net.Interoperability
{
    /// <summary>
    /// Defines a Frost.Api resource methods
    /// </summary>
    public interface IFrostResource : IDisposable
    {
		/// <summary>
		/// Returns raw handle of the api resource
		/// </summary>
		public IntPtr Handle { get; }

        /// <summary>
        /// Returns reference count of the api resource
        /// </summary>
        public ulong ReferenceCount { get; }
    }
}
