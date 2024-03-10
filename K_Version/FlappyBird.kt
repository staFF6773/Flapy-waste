import java.awt.Color
import java.awt.Dimension
import java.awt.Font
import java.awt.Graphics
import java.awt.event.ActionEvent
import java.awt.event.ActionListener
import java.awt.event.KeyEvent
import java.awt.event.KeyListener
import javax.swing.JFrame
import javax.swing.JPanel
import javax.swing.Timer

class FlappyBird : JPanel(), ActionListener, KeyListener {

    private var birdY = 250
    private var birdVelocity = 0
    private var pipesX = 600
    private var topPipeHeight = 200
    private var bottomPipeHeight = 200
    private var pipeGap = 150
    private var score = 0
    private var gameStarted = false

    private val timer: Timer

    init {
        timer = Timer(20, this)
        timer.start()
        addKeyListener(this)
        isFocusable = true
        preferredSize = Dimension(600, 500)
    }

    override fun paintComponent(g: Graphics) {
        super.paintComponent(g)
        g.color = Color.cyan
        g.fillRect(0, 0, width, height)

        g.color = Color.green
        g.fillRect(pipesX, 0, 50, topPipeHeight)
        g.fillRect(pipesX, height - bottomPipeHeight, 50, bottomPipeHeight)

        g.color = Color.red
        g.fillRect(100, birdY, 30, 30)

        g.color = Color.black
        g.setFont(Font("Arial", Font.BOLD, 20))
        g.drawString("Score: $score", 10, 20)

        if (!gameStarted) {
            g.drawString("Press SPACE to start", 200, 250)
        }
    }

    override fun actionPerformed(e: ActionEvent) {
        if (gameStarted) {
            birdVelocity += 2
            birdY += birdVelocity

            pipesX -= 2
            if (pipesX + 50 < 0) {
                pipesX = 600
                topPipeHeight = (Math.random() * 300).toInt()
                bottomPipeHeight = 500 - topPipeHeight - pipeGap
            }

            if (birdY > height || birdY < 0 || (birdY < topPipeHeight && pipesX < 130 && pipesX + 50 > 100) || (birdY + 30 > height - bottomPipeHeight && pipesX < 130 && pipesX + 50 > 100)) {
                gameOver()
            }

            if (pipesX == 100) {
                score++
            }
        }

        repaint()
    }

    private fun gameOver() {
        timer.stop()
        score = 0
        birdY = 250
        birdVelocity = 0
        pipesX = 600
        topPipeHeight = 200
        bottomPipeHeight = 200
        gameStarted = false
        repaint()
    }

    override fun keyPressed(e: KeyEvent) {
        if (e.keyCode == KeyEvent.VK_SPACE) {
            gameStarted = true
            birdVelocity = -10
        }
    }

    override fun keyTyped(e: KeyEvent) {}

    override fun keyReleased(e: KeyEvent) {}

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            val frame = JFrame("Flappy Bird")
            val game = FlappyBird()
            frame.add(game)
            frame.pack()
            frame.isResizable = false
            frame.defaultCloseOperation = JFrame.EXIT_ON_CLOSE
            frame.setLocationRelativeTo(null)
            frame.isVisible = true
            game.requestFocus()
        }
    }
}
