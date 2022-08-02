#pragma once
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[0] <= 74.57499694824219) {
                            if (x[0] <= 68.34000015258789) {
                                return 0;
                            }

                            else {
                                if (x[0] <= 71.78499984741211) {
                                    return 1;
                                }

                                else {
                                    if (x[1] <= 0.8169469237327576) {
                                        return 0;
                                    }

                                    else {
                                        return 0;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.6150944828987122) {
                                return 2;
                            }

                            else {
                                if (x[0] <= 89.91500091552734) {
                                    if (x[1] <= 0.7899510264396667) {
                                        return 2;
                                    }

                                    else {
                                        return 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 0.6413403451442719) {
                                        return 1;
                                    }

                                    else {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }

                protected:
                };
            }
        }
    }