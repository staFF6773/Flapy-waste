using System;
using System.Drawing;
using System.Windows.Forms;
using System.Timers;

namespace FlappyBird
{
    public partial class GameForm : Form
    {
        private const int Gravity = 2;
        private const int JumpSpeed = -20;
        private const int PipeSpeed = -5;
        private const int PipeWidth = 50;
        private const int PipeGap = 200;
        private const int PipeDistance = 300;

        private int score;
        private Rectangle bird;
        private Rectangle[] pipes;
        private System.Timers.Timer gameTimer;

        public GameForm()
        {
            InitializeComponent();
            InitializeGame();
        }

        private void InitializeGame()
        {
            score = 0;
            bird = new Rectangle(50, 200, 20, 20);
            pipes = new Rectangle[2];
            pipes[0] = new Rectangle(300, 0, PipeWidth, 200);
            pipes[1] = new Rectangle(300, pipes[0].Height + PipeGap, PipeWidth, this.Height - pipes[0].Height - PipeGap);

            gameTimer = new System.Timers.Timer(20);
            gameTimer.Elapsed += GameTimer_Tick;
            gameTimer.Enabled = true;
        }

        private void GameTimer_Tick(object sender, ElapsedEventArgs e)
        {
            MoveBird();
            MovePipes();
            CheckCollision();
            this.Invalidate();
        }

        private void MoveBird()
        {
            bird.Y += Gravity;
        }

        private void MovePipes()
        {
            for (int i = 0; i < pipes.Length; i++)
            {
                pipes[i].X += PipeSpeed;

                if (pipes[i].X + PipeWidth < 0)
                {
                    pipes[i].X = this.Width;
                    if (i == 0)
                    {
                        pipes[i].Y = 0;
                        pipes[i].Height = new Random().Next(50, this.Height - PipeGap);
                    }
                    else
                    {
                        pipes[i].Y = pipes[0].Height + PipeGap;
                        pipes[i].Height = this.Height - pipes[i].Y;
                    }
                }

                if (bird.IntersectsWith(pipes[i]))
                {
                    gameTimer.Stop();
                    MessageBox.Show("Game Over! Your score: " + score);
                    InitializeGame();
                    break;
                }

                if (pipes[i].X == bird.X)
                {
                    score++;
                }
            }
        }

        private void CheckCollision()
        {
            if (bird.Y < 0 || bird.Y + bird.Height > this.Height)
            {
                gameTimer.Stop();
                MessageBox.Show("Game Over! Your score: " + score);
                InitializeGame();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;
            g.FillRectangle(Brushes.LightBlue, bird);
            foreach (Rectangle pipe in pipes)
            {
                g.FillRectangle(Brushes.Green, pipe);
            }
            g.DrawString("Score: " + score, Font, Brushes.Black, new Point(10, 10));
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (e.KeyCode == Keys.Space)
            {
                bird.Y += JumpSpeed;
            }
        }
    }
}
