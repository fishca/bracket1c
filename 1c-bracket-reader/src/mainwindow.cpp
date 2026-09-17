#include "mainwindow.h"
#include "bracketparser.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mTreeView(nullptr)
    , mTreeModel(nullptr)
    , mStatusBar(nullptr)
    , mPathLabel(nullptr)
{
    setupUI();
    setupMenu();
    
    setWindowTitle(tr("1C Bracket Format Reader"));
    resize(800, 600);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // Create central widget and layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // Create tree view
    mTreeView = new QTreeView(this);
    mTreeView->setHeaderHidden(true);
    mTreeView->setExpandsOnDoubleClick(true);
    mTreeView->setAlternatingRowColors(true);
    
    // Create model
    mTreeModel = new TreeModel(this);
    mTreeView->setModel(mTreeModel);
    
    // Connect selection change signal
    connect(mTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onSelectionChanged);
    
    layout->addWidget(mTreeView);
    
    // Create status bar
    mStatusBar = statusBar();
    mPathLabel = new QLabel(this);
    mPathLabel->setText(tr("Выберите файл для загрузки"));
    mStatusBar->addWidget(mPathLabel);
}

void MainWindow::setupMenu()
{
    QMenuBar *menuBar = this->menuBar();
    
    QMenu *fileMenu = menuBar->addMenu(tr("Файл"));
    
    QAction *openAction = new QAction(tr("Открыть..."), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Открыть файл 1C"),
        QString(),
        tr("1C Files (*.*)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Ошибка"), 
                             tr("Не удалось открыть файл: %1").arg(file.errorString()));
        return;
    }
    
    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif
    QString content = in.readAll();
    file.close();
    
    // Parse the file
    BracketParser parser;
    auto rootNode = parser.parse(content);
    
    if (rootNode) {
        mTreeModel->setRootNode(std::move(rootNode));
        mTreeView->expandToDepth(2);
        mStatusBar->showMessage(tr("Файл успешно загружен: %1").arg(fileName), 5000);
    } else {
        QMessageBox::warning(this, tr("Предупреждение"), 
                            tr("Не удалось разобрать файл"));
    }
}

void MainWindow::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    
    if (current.isValid()) {
        TreeNode *node = mTreeModel->nodeFromIndex(current);
        if (node) {
            mPathLabel->setText(node->fullPath());
        }
    } else {
        mPathLabel->clear();
    }
}
