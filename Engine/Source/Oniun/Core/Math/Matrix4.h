#pragma once

namespace Onu
{
    struct Matrix4
    {
        union
        {
            struct
            {
                // Column 1
                float M11;
                float M21;
                float M31;
                float M41;

                // Column 1
                float M12;
                float M22;
                float M32;
                float M42;

                // Column 1
                float M13;
                float M23;
                float M33;
                float M43;

                // Column 1
                float M14;
                float M24;
                float M34;
                float M44;
            };

            float Raw[4][4];
        };
    };
}
