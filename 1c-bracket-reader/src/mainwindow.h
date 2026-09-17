#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include "treemodel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void setupUI();
    void setupMenu();
    
    QTreeView *mTreeView;
    TreeModel *mTreeModel;
    QStatusBar *mStatusBar;
    QLabel *mPathLabel;
};

#endif // MAINWINDOW_H
