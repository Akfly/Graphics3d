
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on november of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PIXEL_BUFFER_HEADER
#define PIXEL_BUFFER_HEADER

    namespace aut
    {

        class Pixel_Buffer
        {
        protected:

            size_t width;
            size_t height;

        public:

            Pixel_Buffer(size_t width, size_t height)
            :
                width (width ),
                height(height)
            {
            }

        public:

            size_t get_width () const
            {
                return (width);
            }

            size_t get_height () const
            {
                return (height);
            }

            int offset_at (int x, int y) const
            {
                return (y * width + x);
            }

        public:

            virtual int  bits_per_pixel () const = 0;
            virtual void set_color      (int r, int  g, int b) = 0;
            virtual void set_pixel      (int x, int  y) = 0;
            virtual void set_pixel      (size_t offset) = 0;
            virtual void gl_draw_pixels (int raster_x, int raster_y) const = 0;

        };

    }

#endif
