/****************************************************************************/
/**
 *  @file Key.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Key.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#pragma once

namespace kvs
{

struct Key
{
    enum ASCIICode
    {
        // Symbol keys
        Escape           = 0x1B, ///< escape
        BackSpace        = 0x08, ///< back space
        Tab              = 0x09, ///< horizontal tab
        Space            = 0x20, ///< space
        Exclamation      = 0x21, ///< '!'
        DoubleQuote      = 0x22, ///< '"'
        NumberSign       = 0x23, ///< '#'
        Dollar           = 0x24, ///< '$'
        Percent          = 0x25, ///< '%'
        Ampersand        = 0x26, ///< '&'
        Apostrophe       = 0x27, ///< '''
        ParenthesisLeft  = 0x28, ///< '('
        ParenthesisRight = 0x29, ///< ')'
        Asterisk         = 0x2a, ///< '*'
        Plus             = 0x2b, ///< '+'
        Comma            = 0x2c, ///< ','
        Minus            = 0x2d, ///< '-'
        Period           = 0x2e, ///< '.'
        Slash            = 0x2f, ///< '/'
        Colon            = 0x3a, ///< ':'
        Semicolon        = 0x3b, ///< ';'
        Less             = 0x3c, ///< '<'
        Equal            = 0x3d, ///< '='
        Greater          = 0x3e, ///< '>'
        Question         = 0x3f, ///< '?'
        At               = 0x40, ///< '@'
        BracketLeft      = 0x5b, ///< '['
        Backslash        = 0x5c, ///< '\'
        BracketRight     = 0x5d, ///< ']'
        Circum           = 0x5e, ///< '^'
        Underscore       = 0x5f, ///< '_'
        QuoteLeft        = 0x60, ///< '`'
        BraceLeft        = 0x7b, ///< '{'
        Bar              = 0x7c, ///< '|'
        BraceRight       = 0x7d, ///< '}'
        Tilde            = 0x7e, ///< '~'

        // Number keys
        Zero  = 0x30,
        One   = 0x31,
        Two   = 0x32,
        Three = 0x33,
        Four  = 0x34,
        Five  = 0x35,
        Six   = 0x36,
        Seven = 0x37,
        Eight = 0x38,
        Nine  = 0x39,

        // Alphabet keys
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4a,
        K = 0x4b,
        L = 0x4c,
        M = 0x4d,
        N = 0x4e,
        O = 0x4f,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5a,

        a = 0x61,
        b = 0x62,
        c = 0x63,
        d = 0x64,
        e = 0x65,
        f = 0x66,
        g = 0x67,
        h = 0x68,
        i = 0x69,
        j = 0x6a,
        k = 0x6b,
        l = 0x6c,
        m = 0x6d,
        n = 0x6e,
        o = 0x6f,
        p = 0x70,
        q = 0x71,
        r = 0x72,
        s = 0x73,
        t = 0x74,
        u = 0x75,
        v = 0x76,
        w = 0x77,
        x = 0x78,
        y = 0x79,
        z = 0x7a
    };

    enum SpecialCode
    {
        // Function keys
        F1  = 0x01000010,
        F2  = 0x01000011,
        F3  = 0x01000012,
        F4  = 0x01000013,
        F5  = 0x01000014,
        F6  = 0x01000015,
        F7  = 0x01000017,
        F8  = 0x01000018,
        F9  = 0x01000019,
        F10 = 0x0100001a,
        F11 = 0x0100001b,
        F12 = 0x0100001c,

        // Directional keys
        Left     = 0x01000000,
        Right    = 0x01000001,
        Up       = 0x01000002,
        Down     = 0x01000003,
        PageUp   = 0x01000004,
        PageDown = 0x01000005,
        Home     = 0x01000006,
        End      = 0x01000007,
        Insert   = 0x01000008
    };

    enum Modifier
    {
        NoModifier      = 0x00000000,
        ShiftModifier   = 0x02000000,
        ControlModifier = 0x04000000,
        AltModifier     = 0x08000000
    };
};

} // end of namespace kvs
