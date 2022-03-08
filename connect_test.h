#ifndef CONNECT_TEST_H
#define CONNECT_TEST_H
#include <QObject>
#include <QDebug>
#include "hy_pelco_d_protocol.h"


class connect_test: public QObject
{
    Q_OBJECT
public:
    connect_test();
public slots:
    void test_print();
};

#endif // CONNECT_TEST_H
